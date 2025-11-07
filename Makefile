prg=game
flg=-pthread
obj=main.o monster.o combat.o panel.o

$(prg): $(obj)
	g++ -o $(prg) $(flg) $^

%.o: %.cpp %.h
	g++ -c $<

clean:
	rm -rf $(obj) $(prg)

.PHONY: clean