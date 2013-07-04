if exists('did_thesaurus_vim') || &cp || version < 700 || !has('python')
  finish
endif
let did_thesaurus_vim = 1

let g:thesaurus_path   = get(g:, 'thesaurus_path', '/usr/share/myspell/dicts')
let g:thesaurus_prefix = get(g:, 'thesaurus_prefix', 'th_')
let g:thesaurus_suffix = get(g:, 'thesaurus_suffix', '_v2')
let g:thesaurus_lang   = get(g:, 'thesaurus_lang', 'en_US')

python << ENDPYTHON
import os
import sys

instances = {}
sys.path.insert(0, os.path.join(vim.eval('expand("<sfile>:h:h")'), 'lib'))

class ThesaurusManager(object):
    def __init__(self):
        import thesaurus
        self.module = thesaurus
        self.instances = {}

    def lookup(self, lang, word):
        if not lang in self.instances:
            name = os.path.join(
                vim.eval('g:thesaurus_path'),
                vim.eval('g:thesaurus_prefix') + lang + vim.eval('g:thesaurus_suffix'))
            if not os.path.exists(name + '.idx') or not os.path.exists(name + '.dat'):
                vim.command("echoerr 'No thesaurus found for {}'".format(
                    lang.replace("'", "''")))
                return
            self.instances[lang] = self.module.Thesaurus(name + '.idx', name + '.dat')
        return list(self.instances[lang].lookup(word.lower()))

    @staticmethod
    def pretty(definitions):
        for defi in definitions:
            yield '{} {}'.format(defi.definition, defi.category)
            for syn in defi.synonyms:
                yield '  {}'.format(syn)
            yield ''

    def clean(self):
        self.instances = {}

thesaurus_manager = ThesaurusManager()
ENDPYTHON

function! s:lookup(lang, word)
python << ENDPYTHON
definitions = thesaurus_manager.lookup(vim.eval('a:lang'), vim.eval('a:word'))
vim.command('let l:count={:d}'.format(len(definitions)))
ENDPYTHON
  if l:count == 0
    echo 'No synonyms found for' a:word . '.'
    return
  endif

  silent! split __Thesaurus__
  setlocal ft=thesaurus
  setlocal buftype=nofile
  setlocal bufhidden=wipe
  setlocal noswapfile
  setlocal nowrap
  setlocal nobuflisted

  nnoremap <buffer> <silent> q :bwipeout!<CR>
  nnoremap <buffer> <silent> <CR> :call <SID>use_synonym()<CR>

python << ENDPYTHON
vim.command('call append(0, [{}])'.format(
    ','.join("'{}'".format(line.replace("'", "''"))
              for line in thesaurus_manager.pretty(definitions))
))
ENDPYTHON

  call append(0, [a:word . ' (query)', ''])
  call append(0, 'Usage: q -> close, Enter -> yank synonym')
  silent! normal! ddgg
  setlocal nomodifiable
endfunction

function! s:use_synonym()
  let line = getline('.')
  normal! _v
  if line =~ '('
    normal! t(ge
  else
    normal! g_
  endif
  normal y

  bwipeout!
endfunction

function! s:magic_lookup(...)
  let word = expand('<cword>')
  let lang = get(b:, 'thesaurus_lang', g:thesaurus_lang)
  let args = copy(a:000)

  if a:0 >= 1
    if a:1 =~? '[a-z]\{2}_[a-z]\{2}'
      let lang = a:1
      call remove(args, 0)
    endif
  endif

  if !empty(args)
    let word = join(args)
  endif

  call s:lookup(lang, word)
endf

command! -nargs=* Thesaurus call s:magic_lookup(<f-args>)
