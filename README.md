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
## blacklist
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
## blacklistadd

## blacklistrm