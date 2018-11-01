# eosprofile

Description

This is a skeleton contract that will "listen" to "transfer" actions on the "mytokenaccnt" ( replace with any account hosting an eosio token contract, i.e. eosio.token ) account.

I call methods that execute after listening to events "call-back" functions, therefore I will prefix these methods with "cb" for "call-back".

Features
----
* Blacklist accounts from sending you specific tokens
* Lock tokens from being transferred for either 7 days, 30 days, 90 days, 180 days, or 360 days
  - catch 'transfer' action and check my balance against my lock before sending

Tables
### blacklist
```bash
cleos get table timmy timmy blacklist

{
  "rows": [{
      "account": "sammy",
      "tokens": ["EOS", "ETH", "BTC"]
    }
  ],
  "more": false
}
```


Actions
### blacklistadd
Blocks an account from sending you less than any specified token.  This can be commonly used to prevent spam token transfers.

Inputs:
"name": Account to blacklist
"amount": Smallest amount of tokens you allow to be sent to you by the sender.  If zero '0.0000' then block all amounts

ex: block all token transfers from "evilman" that are under "5.0000 EOS"
```bash
cleos push action timmy blacklistadd '["evilman","5.0000 EOS"]' -p timmy@active
```

## blacklistrm
```bash
cleos push action timmy blacklistrm '["bestfriend"]' -p timmy@active
```
