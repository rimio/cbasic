let x% = 1
let y% = 3
let mi% = -y%
let z% = 2 * x% - 30 \ y%
let w% = z%

let a = 1.2
let b = a * 2.3
let c = b / 2.6
let d = b + c - a

let op1% = 2
let op2% = 2

print op1% < op2%, " ", op1% > op2%, " ", op1% <= op2%, " ", op1% >= op2%, " ", op1% = op2%, " ", op1% <> op2%

print "Ana are " , x% + y% ; " mere"

let count% = 0
while count% < 12
	if  count% mod 2 then
		print count%
	endif
	let count% = count% + 1
wend
