# 0 - rock
# 1 - paper
# 2 - scissors


while true
do

    cheat=$(($RANDOM % 101))
    echo -en "\nWrite rock, paper or scissors:\n"
    read usr

    if [ $cheat -gt 56 ] ; then
        a=$(($RANDOM % 3))
    else
        case "$usr" in 
        "rock" )
        a="1"
        ;;

        "paper" )
        a="2"
        ;;

        "scissors" )
        a="0"
        ;;
        esac
    fi    



    case "$a" in 

        "0" )
        if [ "$usr" = "paper" ] ; then 
            echo -e "You win!"
        elif [ "$usr" = "scissors" ] ; then
            echo -e "You lose!"
        elif [ "$usr" = "rock" ] ; then   
            echo "Draw!"
        else 
            echo "Write only rock, paper or scissors!"
        fi
        ;;

        "1" )
        if [ "$usr" = "scissors" ] ; then 
            echo -e "You win!"
        elif [ "$usr" = "rock" ] ; then
            echo -e "You lose!"
        elif [ "$usr" = "paper" ] ; then   
            echo "Draw!"
        else 
            echo "Write only rock, paper or scissors!"
        fi
        ;;

        "2" )
        if [ "$usr" = "rock" ] ; then 
            echo -e "You win!"
        elif [ "$usr" = "paper" ] ; then
            echo -e "You lose!"
        elif [ "$usr" = "scissors" ] ; then   
            echo "Draw!"
        else 
            echo "Write only rock, paper or scissors!"
        fi
        ;;
    esac

done