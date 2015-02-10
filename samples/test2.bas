let x% = 22
let y% = 0

print x%, " ", not x%
print y%, " ", not y%

let a = 1.5
let b = 1.3

print a, " + ", b, " = ", a + b

print a, " < ", b, " = ", a < b
print a, " > ", b, " = ", a > b
print a, " <= ", b, " = ", a <= b
print a, " >= ", b, " = ", a >= b
print a, " == ", b, " = ", a = b
print a, " != ", b, " = ", a <> b

let count% = 0
while count% < 12
	if  count% mod 2 then
		print count%
	endif
	let count% = count% + 1
wend
