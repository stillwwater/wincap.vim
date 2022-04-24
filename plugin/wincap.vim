if exists('g:wincap_loaded')
  finish
endif

let g:wincap_loaded = 1

if !exists('g:wincap_color')
  let g:wincap_color = ''
endif

if !exists('g:wincap_vim_exe')
  let g:wincap_vim_exe = 'gvim.exe'
endif

if !exists('g:wincap_plugin_exe')
  if exists('g:plug_home')
    let g:wincap_plugin_exe = g:plug_home . '\wincap.vim\bin\wincap.exe'
  else
    let g:wincap_plugin_exe = '~\vimfiles\plugin\wincap.vim\bin\wincap.exe'
  endif
endif

function! Wincap()
    let s:wincap_bg = synIDattr(hlID('Normal'), 'bg')[1:]
    execute 'silent !start /b cmd /c ' . g:wincap_plugin_exe . ' '
        \ . g:wincap_vim_exe . ' '
        \ . (g:wincap_color != '' ? g:wincap_color : s:wincap_bg)
endfunction

if has('win32')
  autocmd VimEnter * call Wincap()
endif
