tudo: 
	pdflatex doc.tex
	bibtex doc
	pdflatex doc.tex
	pdflatex doc.tex
run: tudo
	open *.pdf
clean:
	rm -rf *.aux *.blg *.bbl *.dvi *.lof *.toc *.log *.ps
