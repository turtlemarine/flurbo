// Get last updates on http://flurbo.cc
//             ___ _  _ ____
//              |  |--| |===
//  _____ __    _____ _____ _____ _____
// |   __|  |  |  |  | __  | __  |     |
// |   __|  |__|  |  |    -| __ -|  |  |
// |__|  |_____|_____|__|__|_____|_____|
//   ___  ____ ____    _ ____ ____ ___
//   |--' |--< [__] ___| |=== |___  |
//
// Based on "The Monero Project" and "Cryptonote Protocol".
//
// Copyright (c) 2018-.... The Flurbo developers.
// Copyright (c) 2014-2018 The Monero developers.
// Copyright (c) 2012-2013 The Cryptonote developers.
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include "include_base_utils.h"

using namespace epee;

#include "checkpoints.h"

#include "common/dns_utils.h"
#include "include_base_utils.h"
#include "string_tools.h"
#include "storages/portable_storage_template_helper.h" // epee json include
#include "serialization/keyvalue_serialization.h"

#undef FLURBO_DEFAULT_LOG_CATEGORY
#define FLURBO_DEFAULT_LOG_CATEGORY "checkpoints"

namespace cryptonote
{
  /**
   * @brief struct for loading a checkpoint from json
   */
  struct t_hashline
  {
    uint64_t height; //!< the height of the checkpoint
    std::string hash; //!< the hash for the checkpoint
        BEGIN_KV_SERIALIZE_MAP()
          KV_SERIALIZE(height)
          KV_SERIALIZE(hash)
        END_KV_SERIALIZE_MAP()
  };

  /**
   * @brief struct for loading many checkpoints from json
   */
  struct t_hash_json {
    std::vector<t_hashline> hashlines; //!< the checkpoint lines from the file
        BEGIN_KV_SERIALIZE_MAP()
          KV_SERIALIZE(hashlines)
        END_KV_SERIALIZE_MAP()
  };

  //---------------------------------------------------------------------------
  checkpoints::checkpoints()
  {
  }
  //---------------------------------------------------------------------------
  bool checkpoints::add_checkpoint(uint64_t height, const std::string& hash_str)
  {
    crypto::hash h = crypto::null_hash;
    bool r = epee::string_tools::parse_tpod_from_hex_string(hash_str, h);
    CHECK_AND_ASSERT_MES(r, false, "Failed to parse checkpoint hash string into binary representation!");

    // return false if adding at a height we already have AND the hash is different
    if (m_points.count(height))
    {
      CHECK_AND_ASSERT_MES(h == m_points[height], false, "Checkpoint at given height already exists, and hash for new checkpoint was different!");
    }
    m_points[height] = h;
    return true;
  }
  //---------------------------------------------------------------------------
  bool checkpoints::is_in_checkpoint_zone(uint64_t height) const
  {
    return !m_points.empty() && (height <= (--m_points.end())->first);
  }
  //---------------------------------------------------------------------------
  bool checkpoints::check_block(uint64_t height, const crypto::hash& h, bool& is_a_checkpoint) const
  {
    auto it = m_points.find(height);
    is_a_checkpoint = it != m_points.end();
    if(!is_a_checkpoint)
      return true;

    if(it->second == h)
    {
      MINFO("CHECKPOINT PASSED FOR HEIGHT " << height << " " << h);
      return true;
    }else
    {
      MWARNING("CHECKPOINT FAILED FOR HEIGHT " << height << ". EXPECTED HASH: " << it->second << ", FETCHED HASH: " << h);
      return false;
    }
  }
  //---------------------------------------------------------------------------
  bool checkpoints::check_block(uint64_t height, const crypto::hash& h) const
  {
    bool ignored;
    return check_block(height, h, ignored);
  }
  //---------------------------------------------------------------------------
  //FIXME: is this the desired behavior?
  bool checkpoints::is_alternative_block_allowed(uint64_t blockchain_height, uint64_t block_height) const
  {
    if (0 == block_height)
      return false;

    auto it = m_points.upper_bound(blockchain_height);
    // Is blockchain_height before the first checkpoint?
    if (it == m_points.begin())
      return true;

    --it;
    uint64_t checkpoint_height = it->first;
    return checkpoint_height < block_height;
  }
  //---------------------------------------------------------------------------
  uint64_t checkpoints::get_max_height() const
  {
    std::map< uint64_t, crypto::hash >::const_iterator highest =
        std::max_element( m_points.begin(), m_points.end(),
                         ( boost::bind(&std::map< uint64_t, crypto::hash >::value_type::first, _1) <
                           boost::bind(&std::map< uint64_t, crypto::hash >::value_type::first, _2 ) ) );
    return highest->first;
  }
  //---------------------------------------------------------------------------
  const std::map<uint64_t, crypto::hash>& checkpoints::get_points() const
  {
    return m_points;
  }

  bool checkpoints::check_for_conflicts(const checkpoints& other) const
  {
    for (auto& pt : other.get_points())
    {
      if (m_points.count(pt.first))
      {
        CHECK_AND_ASSERT_MES(pt.second == m_points.at(pt.first), false, "Checkpoint at given height already exists, and hash for new checkpoint was different!");
      }
    }
    return true;
  }

  bool checkpoints::init_default_checkpoints(bool testnet)
  {
    if (testnet)
    {
      ADD_CHECKPOINT(1000000, "46b690b710a07ea051bc4a6b6842ac37be691089c0f7758cfeec4d5fc0b4a258");
      return true;
    }
    ADD_CHECKPOINT(0, "07ea79d1bfc623fcb1544f64915ce522b736eef47154d2b7a816ed992f3b5caa");
    ADD_CHECKPOINT(10, "ad221cf246115c0b152b26ddd83ea350c0ccdd2d41e192486eac684ffc81a7b1");
    ADD_CHECKPOINT(100, "c33b39fef3d706825cb9326f8b86c38eb112f0c5220f9275f34f9280df20886b");
    ADD_CHECKPOINT(200, "d1b045c90888642224783a4d4892ab7427bc653363e20c5a9861f735cbc1cf62");
    ADD_CHECKPOINT(300, "fa54c5ec2cd30a50ba822c6fc564779069235a2daecba471be1a319bf1ed9935");
    ADD_CHECKPOINT(400, "edd072f4eca38a219559d2719d5eeea3e14b17d100ea029b3a3995d79ca40277");
    ADD_CHECKPOINT(500, "e6d7702e38a53f39352f84bf567ea23d3e4f261f543c8d0360769412e4a4479b");
    ADD_CHECKPOINT(600, "6ab40fb1b9a7c2e7c1d596d6b99d821a81d1b888ce38db746b2a62663c46ebdd");
    ADD_CHECKPOINT(700, "2f803686c0d7192c6b685a4267f5e74f3beb75024c5d796fc176acbd93f8d3fc");
    ADD_CHECKPOINT(800, "d6f839d24b1d6c36cce2785a1bf92a07514481bb2a2cdef04766af65c266e21a");
    ADD_CHECKPOINT(900, "6c185ed30533b9a60569e331d6ea508bf942c485e346d934359c371189c5a52d");
    ADD_CHECKPOINT(1000, "20ff312e33d2b25dbb6efd989f01e7ded5faba4a44245080fc63a82ad1da0c6a");
    ADD_CHECKPOINT(1100, "8c19a6dc7d14de81c20d0d81c3df32cdde1b0804dba874a54fe5b7b7e3c32605");
    ADD_CHECKPOINT(1200, "5bdcf62954950be1fa4106062c8c84fc2c256c0666c5914e1f6d892931e3bf70");
    ADD_CHECKPOINT(1300, "fbdcfb2c3ff86cf616ccaba841bf1e94bb3b7b36bcec4785cdee39a1e9ba549c");
    ADD_CHECKPOINT(1400, "8e2ad062218ebb8d4e54a647cef958add29ae13b084c1a6eaf40f7403e3a8fb0");
    ADD_CHECKPOINT(1500, "dd9fab412ae181dca62e398cdec51a2c6014c7b237f246892dee4faac9fad05a");
    ADD_CHECKPOINT(1600, "ca67884a2b53b7026c04fc0a332d6c671dbcc000f6cc2baf2a8b0dee6da7b14c");
    ADD_CHECKPOINT(1700, "0ba79b6b0cf3ca370581f2371d50b99c1a91a2643591b774a9bfdfee9ee71a9c");
    ADD_CHECKPOINT(1800, "94d26312c9980dfd8b4b4e2bef724442567c4c89ff5e5518010f66895532a799");
    ADD_CHECKPOINT(1900, "c9fde91a2179ba1df79c666b540f25b95274f79f82e04337589aff69c53a93fa");
    ADD_CHECKPOINT(2000, "e1a2949d99186229bf8dbc7616179b9f27356338e36ad05945336425544381fc");
    ADD_CHECKPOINT(2100, "bd096a455e38349436a7a427564170f8663c2d3cae061c3ef3f25430a02e1ea6");
    ADD_CHECKPOINT(2200, "dd08225625ae6ed639a7000c5f56e79e0bde01b3e61d9f005ccd8e372231ab3d");
    ADD_CHECKPOINT(2300, "a53c61df93590660819ff81d187a4a0f51e33a15173f977f32011a2ec0f9ec2c");
    ADD_CHECKPOINT(2400, "33e65a5574ef7c0dd812952c4caa09bb456b35aab8d238618634e76fab88ae96");
    ADD_CHECKPOINT(2500, "ac278b919c9b52ffae417fe12f8b8e58a4b5f871b093c8a81ea253920a80658a");


    return true;
  }

  bool checkpoints::load_checkpoints_from_json(const std::string &json_hashfile_fullpath)
  {
    boost::system::error_code errcode;
    if (! (boost::filesystem::exists(json_hashfile_fullpath, errcode)))
    {
      LOG_PRINT_L1("Blockchain checkpoints file not found");
      return true;
    }

    LOG_PRINT_L1("Adding checkpoints from blockchain hashfile");

    uint64_t prev_max_height = get_max_height();
    LOG_PRINT_L1("Hard-coded max checkpoint height is " << prev_max_height);
    t_hash_json hashes;
    if (!epee::serialization::load_t_from_json_file(hashes, json_hashfile_fullpath))
    {
      MERROR("Error loading checkpoints from " << json_hashfile_fullpath);
      return false;
    }
    for (std::vector<t_hashline>::const_iterator it = hashes.hashlines.begin(); it != hashes.hashlines.end(); )
    {
      uint64_t height;
      height = it->height;
      if (height <= prev_max_height) {
	LOG_PRINT_L1("ignoring checkpoint height " << height);
      } else {
	std::string blockhash = it->hash;
	LOG_PRINT_L1("Adding checkpoint height " << height << ", hash=" << blockhash);
	ADD_CHECKPOINT(height, blockhash);
      }
      ++it;
    }

    return true;
  }

  bool checkpoints::load_checkpoints_from_dns(bool testnet)
  {
    std::vector<std::string> records;

    // All four FlurboPulse domains have DNSSEC on and valid
    static const std::vector<std::string> dns_urls = { "checkpoints.flurbopulse.se"
						     , "checkpoints.flurbopulse.org"
						     , "checkpoints.flurbopulse.net"
						     , "checkpoints.flurbopulse.co"
    };

    static const std::vector<std::string> testnet_dns_urls = { "testpoints.flurbopulse.se"
							     , "testpoints.flurbopulse.org"
							     , "testpoints.flurbopulse.net"
							     , "testpoints.flurbopulse.co"
    };

    if (!tools::dns_utils::load_txt_records_from_dns(records, testnet ? testnet_dns_urls : dns_urls))
      return true; // why true ?

    for (const auto& record : records)
    {
      auto pos = record.find(":");
      if (pos != std::string::npos)
      {
        uint64_t height;
        crypto::hash hash;

        // parse the first part as uint64_t,
        // if this fails move on to the next record
        std::stringstream ss(record.substr(0, pos));
        if (!(ss >> height))
        {
    continue;
        }

        // parse the second part as crypto::hash,
        // if this fails move on to the next record
        std::string hashStr = record.substr(pos + 1);
        if (!epee::string_tools::parse_tpod_from_hex_string(hashStr, hash))
        {
    continue;
        }

        ADD_CHECKPOINT(height, hashStr);
      }
    }
    return true;
  }

  bool checkpoints::load_new_checkpoints(const std::string &json_hashfile_fullpath, bool testnet, bool dns)
  {
    bool result;

    result = load_checkpoints_from_json(json_hashfile_fullpath);
    if (dns)
    {
      result &= load_checkpoints_from_dns(testnet);
    }

    return result;
  }
}
