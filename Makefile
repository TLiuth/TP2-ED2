# Makefile at <TP2-ED2>
.PHONY: build

# Arquivos
source_files	:= $(wildcard *.c)
header_files	:= $(wildcard *.h)
executable		:= ordena

# Construção
build: $(executable)
	@echo Construído...

$(executable): $(source_files) $(header_files)
	gcc $(source_files) -o $@ -Wall -Wextra -pedantic -O3