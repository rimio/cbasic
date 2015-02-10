let x% = 22
let y% = 0

print x%, " ", not x%
print y%, " ", not y%

let o1% = 0
while o1% < 2
	let o2% = 0
	while o2% < 2
		print o1%, " and ", o2%, " = ", o1% and o2%
		print o1%, " or  ", o2%, " = ", o1% or  o2%
		print o1%, " xor ", o2%, " = ", o1% xor o2%
		let o2% = o2% + 1
	wend
	let o1% = o1% + 1
wend

let a = 1.5
let b = 1.3
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
