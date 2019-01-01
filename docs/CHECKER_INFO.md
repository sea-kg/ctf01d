## Checker description

### Checker console interface

Usage: ```./checker.py <ip_address> <command> <flag_id> <flag> ```

Where:

  * ip_address - address of machine with this service
  * command - command, can be "put" or "check"
  * flag_id - string (10), id of flag [a-zA-Z0-9]{10}
  * flag - uuid, value of flag [a-z0-9]{8}-[a-z0-9]{4}-[a-z0-9]{4}-[a-z0-9]{4}-[a-z0-9]{12}

Commands:

 * `put` - put flag to service
 * `check` - check flag on service

Call-examples:

 * ```./checker.py 127.0.0.1 put "1q2w3e4r5t" "6a331fd2-133a-4713-9587-12652d34666d"```
 * ```./checker.py 127.0.0.1 check "1q2w3e4r5t" "6a331fd2-133a-4713-9587-12652d34666d"```

### Possible return codes

 * 101 - service is up  (worked fine)
 * 102 - service is corrupt
 * 103 - service is mumble (or checker infinity work)
 * 104 - service is down
 * other - checker is shit