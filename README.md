# Introduction #

The `master` branch of this repository contains a C++ library with Python bindings to read thesaurus
files in the mythes format (see the upstream URL of
[those packages](https://www.archlinux.org/packages/?q=mythes-) for how to obtain them).

This branch contains a VIM plugin that uses the aforementioned library to provide a lookup functionality
via the `:Thesaurus` command.

# Installation #

Using [pathogen.vim](https://github.com/tpope/vim-pathogen):

```
cd ~/.vim/bundle
git clone -b vim https://github.com/kljohann/thesaurus.git
cd thesaurus/lib
./build.sh
```

You will probably need CMake, Python, the boost libraries (and dev headers) and a recent C++ compiler.

# Usage #

`:Thesaurus [LANG] [WORD]` where `LANG` is a language identifier of the form `en_US`, `de_DE`
as used in the mythes file names and `WORD` is the word to look up.
When not specified the word under the cursor and the language specified in `b:thesaurus_lang`
or `g:thesaurus_lang` or `en_US` will be used.
In the opened buffer `<CR>` will yank the synonym on the current line and `q` will abort.

`:ThesaurusEnableCompletion` sets the `completefunc` option so `C-X C-U` can be used to
select a synonym for the current word.

# Options #

 * `g:thesaurus_path` + `g:thesaurus_prefix` + `LANG` + `g:thesaurus_suffix` + `.idx` or `.dat` will be used
   as the path to the thesaurus file.
 * `b:thesaurus_lang` / `g:thesaurus_lang` is the default language.
 * `g:thesaurus_height` is the height of the split window that will be opened.
