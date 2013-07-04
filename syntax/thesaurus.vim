if exists("b:current_syntax")
    finish
endif

syn match   ThesaurusDefinition "^\w.*" contains=ThesaurusCategory
syn match   ThesaurusCategory   "(.\+)$" contained
syn match   ThesaurusSynonym    "^ [^(]\+"
syn match   ThesaurusUsage      "^Usage:.*$"

hi def link ThesaurusDefinition Keyword
hi def link ThesaurusCategory   Type
hi def link ThesaurusSynonym    String
hi def link ThesaurusUsage      Comment

let b:current_syntax = "thesaurus"
