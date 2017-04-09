PARSER_LOC=./parserfiles

default: pass runmem2reg run #parser ll pass runmem2reg run
# Parser not included. Please just use clang to emit llvm

vi:
	vim common-expr-elim.cc

run:
	@echo "+++++++++++++++++++++++++++++++++++++++++++++"
	@echo "++++		After Pass:		++++"
	@echo "+++++++++++++++++++++++++++++++++++++++++++++"
	cat projecttest.ll | opt -mem2reg \
		| opt -load ./common-expr-elim.so -common-expr-elim | llvm-dis

runmem2reg:
	@echo "+++++++++++++++++++++++++++++++++++++++++++++"
	@echo "++++		Mem2Reg Pass:		++++"
	@echo "+++++++++++++++++++++++++++++++++++++++++++++"
	cat projecttest.ll | opt -mem2reg | llvm-dis

parser:
	bison -o ${PARSER_LOC}/parser.c ${PARSER_LOC}/parser.y -d -v
	flex -o ${PARSER_LOC}/scanner.c ${PARSER_LOC}/scanner.l
	g++ ${PARSER_LOC}/parser.c \
		${PARSER_LOC}/scanner.c \
		-o parser \
		`llvm-config --cppflags` \
		`llvm-config --ldflags` \
		-lLLVM-3.4 \
		-std=c++11 \
		${PARSER_LOC}/SymbolTable.cc \
		${PARSER_LOC}/Type.cc

pass:
	g++ -fPIC -shared \
		common-expr-elim.cc \
		-o common-expr-elim.so \
		-std=c++11 \
		`llvm-config --cppflags`
	@echo "+++++++++++++++++++++++++++++++++++++++++++++"
	@echo "++++		Before Pass:		++++"
	@echo "+++++++++++++++++++++++++++++++++++++++++++++"
	cat projecttest.ll
	

ll:
	./parser projecttest.c > projecttest.ll 2>&1



clean:
	rm common-expr-elim.so

cleanparser:
	rm ${PARSER_LOC}/scanner.c ${PARSER_LOC}/parser.h ${PARSER_LOC}/parser.c \
		${PARSER_LOC}/parser.output parser
