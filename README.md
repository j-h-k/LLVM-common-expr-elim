# LLVM-common-expr-elim

## LLVM code that is generated from the parser generates significant counts of repeated expressions. These repeated expressions can be completely eliminated by replacing them with LLVM variables already evaluated.
(keynote: In LLVM, there is no left hand side to an instruction. Simply names for instructions. This is due to the fact that LLVM IR is in SSA form. Distinguishing register and instruction is meaningless.)

## Makefile (w/o parser)

cat projecttest.ll (before pass) ->
	mem2reg pass (mem2reg pass) ->
	common-expr-elim pass (after pass) ->
	llvm-dis


## The pseudocode for common-elim-expr:
```
	for each basicblock:
		for each instruction:
			key = getOpcodeName()
			if key = “alloca” || “br” || “store” || “load”:
				continue
			for each operands:
				key += operands
			if key already exists
				replace expression with existing modified function now
			else
				store expression
		return is modified?
```

# Key Information

The common-expr-elim pass **SHOULD FOLLOW AFTER MEM2REG PASS**.
