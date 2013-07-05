if exists("b:current_syntax")
    finish
endif

syn match   ThesaurusDefinition "^\S.*$" contains=ThesaurusCategory
syn match   ThesaurusCategory   "((\?[^)]\+)\?)$" contained
syn match   ThesaurusSynonym    "^\s.*$" contains=ThesaurusWord
syn match   ThesaurusWord       "\(^\s\|)\)\zs[^(]\+" contained
syn match   ThesaurusUsage      "^Usage:.*$"

hi def link ThesaurusDefinition Keyword
hi def link ThesaurusCategory   Type
hi def link ThesaurusWord       String
hi def link ThesaurusUsage      Comment

let b:current_syntax = "thesaurus"
