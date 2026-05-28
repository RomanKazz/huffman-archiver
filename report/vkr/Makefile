VKR ?= vkr
ENGINE ?= xelatex

.PHONY: all clean

all: $(VKR).pdf

%.pdf: %.tex *.bib *.tex
	latexmk -$(ENGINE) -synctex=1 -interaction=nonstopmode -file-line-error -shell-escape $<

clean:
	latexmk -c $(VKR).tex
	$(RM) *.pdf *.synctex.gz
