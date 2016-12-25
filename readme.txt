Operations that are not bound to last successful login

• login: login to a specific account
	Input: login [ID] [password]

• create: create a specific account with password
	Input: create [ID] [password]

• delete: delete a specific account with password
	Input: delete [ID] [password]

• merge: merge the second account into the first one; after merging, the second one is deleted
	Input: merge [ID1] [password1] [ID2] [password2]


Operations that are bound to last successful login

• deposit: deposit money into [last-successful-login-ID]
	Input: deposit [num]

• withdraw: withdraw money from [last-successful-login-ID]
	Input: withdraw [num]

• transfer: transfer money from [last-successful-login-ID] to a specific account
	Input: transfer [ID] [num]

• find: find all existing account IDs that matches [wildcard ID] but is different from [last-successful-login-ID]
	Input: find [wildcard_ID] // [wildcard ID] is a ID string that may also contain * or ?, where * stands for 0 or more characters, and ? stands for exactly 1 character

• search: search all transfer history of [last-successful-login-ID] from/to a specific account
	Input: search [ID]
