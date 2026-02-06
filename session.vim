let SessionLoad = 1
let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
let v:this_session=expand("<sfile>:p")
let VM_check_mappings =  1 
let VM_mouse_mappings =  0 
let VM_default_mappings =  1 
let VM_persistent_registers =  0 
let VM_highlight_matches = "underline"
let BufferlinePositions = "[\"/home/tcwhite/Projects/C-C++/tui/src/app/app.cpp\",\"/home/tcwhite/Projects/C-C++/tui/src/app/app.hpp\",\"/home/tcwhite/Projects/C-C++/tui/src/chess/game.cpp\",\"/home/tcwhite/Projects/C-C++/tui/src/chess/game.hpp\",\"/home/tcwhite/Projects/C-C++/tui/src/engine/engine.hpp\",\"/home/tcwhite/Projects/C-C++/tui/src/engine/engine.cpp\",\"/home/tcwhite/Projects/C-C++/tui/test.cpp\",\"/home/tcwhite/Projects/C-C++/tui/src/chess/move.hpp\"]"
silent only
silent tabonly
cd ~
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
let s:shortmess_save = &shortmess
if &shortmess =~ 'A'
  set shortmess=aoOA
else
  set shortmess=aoO
endif
badd +55 ~/Projects/C-C++/tui/src/app/app.cpp
badd +20 ~/Projects/C-C++/tui/src/app/app.hpp
badd +1 ~/Projects/C-C++/tui/src/chess/game.cpp
badd +43 ~/Projects/C-C++/tui/src/chess/game.hpp
badd +158 ~/Projects/C-C++/tui/src/engine/engine.cpp
badd +32 ~/Projects/C-C++/tui/src/engine/engine.hpp
badd +7 ~/Projects/C-C++/tui/test.cpp
badd +40 ~/Projects/C-C++/tui/src/chess/move.hpp
argglobal
%argdel
edit ~/Projects/C-C++/tui/src/app/app.cpp
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
let &splitbelow = s:save_splitbelow
let &splitright = s:save_splitright
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
argglobal
balt ~/Projects/C-C++/tui/src/app/app.hpp
setlocal foldmethod=expr
setlocal foldexpr=v:lua.vim.lsp.foldexpr()
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=99
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
let s:l = 55 - ((42 * winheight(0) + 23) / 47)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 55
normal! 0
tabnext 1
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0 && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20
let &shortmess = s:shortmess_save
let &winminheight = s:save_winminheight
let &winminwidth = s:save_winminwidth
let s:sx = expand("<sfile>:p:r")."x.vim"
if filereadable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &g:so = s:so_save | let &g:siso = s:siso_save
set hlsearch
nohlsearch
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
