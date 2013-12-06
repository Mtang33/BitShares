#define BOOST_TEST_MODULE BitSharesTest
#include <boost/test/unit_test.hpp>

#include <bts/blockchain/blockchain_wallet.hpp>
#include <bts/blockchain/blockchain_db.hpp>
#include <bts/blockchain/block.hpp>
#include <fc/reflect/variant.hpp>
#include <fc/filesystem.hpp>
#include <fc/io/json.hpp>
#include <fc/log/logger.hpp>
#include <fc/io/raw.hpp>
#include <bts/config.hpp>

using namespace bts::blockchain;

fc::ecc::private_key test_genesis_private_key()
{
    return fc::ecc::private_key::generate_from_seed( fc::sha256::hash( "genesis", 7 ) );
}

bts::blockchain::trx_block create_test_genesis_block()
{
   bts::blockchain::trx_block b;
   b.version    = 0;
   b.prev       = block_id_type();
   b.block_num  = 0;
   b.timestamp  = fc::time_point::from_iso_string("20131201T054434");

   signed_transaction coinbase;
   coinbase.version = 0;
   coinbase.valid_after = 0;
   coinbase.valid_blocks = 0;

   // TODO: init from PTS here...
   coinbase.outputs.push_back( 
      trx_output( claim_by_signature_output( bts::address(test_genesis_private_key().get_public_key()) ), 100*COIN, asset::bts) );

   b.trxs.emplace_back( std::move(coinbase) );
   b.trx_mroot   = b.calculate_merkle_root();

   return b;
}



BOOST_AUTO_TEST_CASE( bitshares_wallet_test )
{
   try {
     fc::temp_directory temp_dir;
     bts::blockchain::blockchain_db chain;
     chain.open( temp_dir.path() / "chain" );

     auto genesis = create_test_genesis_block();
     ilog( "genesis block: \n${s}", ("s", fc::json::to_pretty_string(genesis) ) );
     chain.push_block( genesis );

     bts::blockchain::wallet  wallet;
     wallet.open( temp_dir.path() / "chain" );

     // load a private key into the wallet
     wallet.import_key( test_genesis_private_key() );
     ilog("scan chain" );
     wallet.scan_chain( chain );
     ilog("dump" );
     wallet.dump();
     wallet.set_fee_rate( chain.get_fee_rate() );

     auto balance = wallet.get_balance(bts::blockchain::asset::bts);

     auto a1      = wallet.get_new_address();
     auto a2      = wallet.get_new_address();
     auto a3      = wallet.get_new_address();
     auto a4      = wallet.get_new_address();

     wallet.set_stake(chain.get_stake());
                  
     auto trx1    = wallet.transfer( asset(20*COIN,asset::bts), a1 );
     wallet.dump();

     std::vector<signed_transaction> trxs;
     trxs.push_back(trx1);

     auto block1 = chain.generate_next_block( trxs );
     chain.push_block( block1 );

     wallet.scan_chain( chain, block1.block_num );
     wallet.dump();

     auto trx2    = wallet.transfer( asset(25*COIN,asset::bts), a2 );
     wallet.dump();

     trxs[0] = trx2;

     auto block2 = chain.generate_next_block( trxs );
     chain.push_block( block2 );

     wallet.scan_chain( chain, block2.block_num );
     wallet.dump();

   //  auto trx2    = wallet.transfer( 2000*COIN, asset::bts, a2 );
   //  auto trx3    = wallet.transfer( 3000*COIN, asset::bts, a3 );
   //  auto trx4    = wallet.transfer( 4000*COIN, asset::bts, a4 );
  } 
  catch ( const fc::exception& e )
  {
     elog( "${e}", ("e", e.to_detail_string() ) );
     throw;
  }
}