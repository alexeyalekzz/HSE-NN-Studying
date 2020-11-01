#!/bin/bash

function parseOperand () {

    if [[ ("$1" =~ ^[AaOo]+[DdTt]+[Ii]+[Nn]?) || ($1 = 1) ]]; then                      # odin/ODIN/aTin/AdIn...
        return 1
    elif [[ ("$1" =~ ^[DdTt]+[Vv]+[Aa]?) || ($1 = 2) ]]; then                           # Tva/Dva/dVa...
        return 2
    elif [[ ("$1" =~ ^[Tt]+[Rr]+[IiEe]?) || ($1 = 3) ]]; then                           # Tri/Tre/Tree...
        return 3
    elif [[ ("$1" =~ ^[Cc]+[Hh]+[EeIi]+[Tt]+[EeIi]+[Rr]+[EeIi]?) || ($1 = 4) ]]; then   # cHiTire/chetire...
        return 4
    elif [[ ("$1" =~ ^[Pp]+[Yy]+[Aa]+[Tt]+(\'|)?) || ($1 = 5) ]]; then                  # pyat/pyat'...
        return 5 
    elif [[ ("$1" =~ ^[Ss]+[Hh]+[Ee]+[Ss]+[Tt]+(\'|)?) || ($1 = 6) ]]; then             # shest/shest'...
        return 6
    elif [[ ("$1" =~ ^[Ss]+[Ee]+[Mm]+(\'|)?) || ($1 = 7) ]]; then                       # sem/sem'...
        return 7
    elif [[ ("$1" =~ ^[Vv]+[Oo]+[Ss]+[EeIi]+[Mm]+(\'|)?) || ($1 = 8) ]]; then           # vosem/vosim'...
        return 8
    elif [[ ("$1" =~ ^[Dd]+[Ee]+[Vv]+[YyAa]+[Tt]+(\'|)?) || ($1 = 9) ]]; then           # devyat'/devat'/devyt...
        return 9
    else 
        return 10
    fi
}

echo "Write expression:"
read a op b

parseOperand $a
a=$?
parseOperand $b
b=$?

if [[ $a != 10 && $b != 10 ]]; then
    echo "Answer is: $(expr $a "$op" $b)"
else
    echo "Can't recognize the digits, try again"
fi
