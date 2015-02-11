let count% = 30
let a% = 1
let b% = 1

while count% > 0
	let count% = count%-1
	print a%
	let b% = a% + b%
	let a% = b% - a%
wend
