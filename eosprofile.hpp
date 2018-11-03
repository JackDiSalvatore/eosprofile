#include <eosiolib/eosio.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/asset.hpp>

#include <set>

#define EOS_SYMBOL S(4,EOS)

namespace eosio {

using std::string;

const account_name TOKEN_ACCOUNT = N(eosio.token);
const account_name TOKEN_ACTION_TRANSFER = N(transfer);

/**
 * FIXME:
 * The actual `eosio.token` transfer struct definition until its definition is accesible
 * from an actual `eosio.token.hpp` file. Until then, we define it ourself so
 * we can unpack the actual action data when a token transfer occurs inside
 * the `eosio.token` contract to this contract's account.
 */
struct token_transfer {
  account_name from;
  account_name to;
  asset quantity;
  std::string memo;
};


class profile : public contract {
public:

   explicit profile( account_name self ) : contract( self ),
                                           _blacklist( self, self ) {}


   // @abi action
   void blacklistadd( account_name account, asset token_to_block );

   // @abi action
   void blacklistrm( account_name account );


   /** CALLBACKS **/

   void cb_transfer() {
      token_transfer action = unpack_action_data<eosio::token_transfer>();
      print("called back triggered\n");

      auto blacklistee = _blacklist.find( action.from );

      if ( blacklistee != _blacklist.end() ) {
         print(eosio::name{action.from}, " is blacklisted\n");

         eosio_assert(false, "sender is blacklisted");
      }
      //eosio_assert( blacklistee == _blacklist.end(), " sender blacklisted");

      if (action.to == _self && action.quantity.symbol == EOS_SYMBOL) {
         print("You received ", action.quantity, " tokens!");
      }
   }


   /** NOTIFCATION **/

   static bool is_token_transfer(uint64_t code, uint64_t action) {
      return /*code == TOKEN_ACCOUNT &&*/ action == TOKEN_ACTION_TRANSFER;
   }

private:

   /// @abi table blacklist i64
   struct blacklist_row {
      account_name                  account;
      std::set<eosio::symbol_name>  tokens_blocked;

      uint64_t primary_key() const { return account; }
   };

   typedef multi_index<N(blacklist), blacklist_row> blacklist_table;

   blacklist_table _blacklist;

};


/**
 * SmartContract C entrypoint using a macro based on the list of action in the.
 * For each of defined action, a switch branch is added
 * automatically unpacking the data into the action's structure and dispatching
 * a method call to `action` define in this SmartContract.
 *
 * Each time a new action is added, EOSIO_API definition should be expanded with the
 * new action handler's method name.
 */
extern "C" {
void apply(uint64_t receiver, uint64_t code, uint64_t action) {
   auto self = receiver;
   if (code == self) {
      // Don't rename `thiscontract`, it's being use verbatim in `EOSIO_API` macro
      profile thiscontract(self);
      switch (action) { EOSIO_API(profile, (blacklistadd)(blacklistrm)) }

      eosio_exit(0);
   }

   if ( profile::is_token_transfer(code, action) ) {
      profile(self).cb_transfer();
      eosio_exit(0);
   }
}
}


}  /// namespace eosio
