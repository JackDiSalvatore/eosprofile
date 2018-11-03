#include "eosprofile.hpp"


namespace eosio {


   void profile::blacklistadd( account_name account, asset token_to_block ) {
      require_auth( _self );

      print(token_to_block, "\n");

      auto blacklistee = _blacklist.find( account );
      // find account in table
      if ( blacklistee == _blacklist.end() ) {
         // if no account create new row
         print("Adding ", eosio::name{ account }, " to blacklist\n");

         blacklistee = _blacklist.emplace( _self, [&]( auto& bl ) {
            print("Blocked ", eosio::name{bl.account} , "\n");
            bl.account = account;
         });
      } else {
         ;   // account already blacklisted
      }

      // extract token name, symbol, and percision
      auto token_to_block_precision = token_to_block.symbol.precision();
      print("Name: ", token_to_block.symbol.name() ,"\n");
      print("Symbol: ", token_to_block.symbol, "\n");
      print("Percision: ", token_to_block_precision, "\n");

      _blacklist.modify( blacklistee, 0, [&]( auto& bl ) {
         auto existing_token = bl.tokens_blocked.find( token_to_block.symbol.name() );

         if( existing_token == bl.tokens_blocked.end() ) {
            print("Token Not Found!\nAdding ", token_to_block.symbol, " as ",
                  token_to_block.symbol.name(), "\n");
            bl.tokens_blocked.emplace( token_to_block.symbol.name() );
         } else {
            print("Token Found! ", *existing_token, " already blacklisted\n");
         }
      });

   }


   void profile::blacklistrm( account_name account ) {
      require_auth( _self );

      auto blacklist_itr = _blacklist.find( account );

      eosio_assert( blacklist_itr != _blacklist.end(),
                    " account not on blacklist");

      const auto& blacklisted_account = *blacklist_itr;

      _blacklist.erase( blacklisted_account );
   }


}  /// namespace eosio
