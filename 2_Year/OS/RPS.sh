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

        case "$a" in 

            "0" )
            if [ "$usr" = "paper" ] ; then 
                echo -e "\e[32mYou win!\e[0m"
            elif [ "$usr" = "scissors" ] ; then
                echo -e "\e[31mYou lose!\e[0m"
            elif [ "$usr" = "rock" ] ; then   
                echo "Draw!"
            else 
                echo "Write only rock, paper or scissors!"
            fi
            ;;

            "1" )
            if [ "$usr" = "scissors" ] ; then 
                echo -e "\e[32mYou win!\e[0m"
            elif [ "$usr" = "rock" ] ; then
                echo -e "\e[31mYou lose!\e[0m"
            elif [ "$usr" = "paper" ] ; then   
                echo "Draw!"
            else 
                echo "Write only rock, paper or scissors!"
            fi
            ;;

            "2" )
            if [ "$usr" = "rock" ] ; then 
                echo -e "\e[32mYou win!\e[0m"
            elif [ "$usr" = "paper" ] ; then
                echo -e "\e[31mYou lose!\e[0m"
            elif [ "$usr" = "scissors" ] ; then   
                echo "Draw!"
            else 
                echo "Write only rock, paper or scissors!"
            fi
            ;;
        esac
    else
        if [ "$usr" = "rock" -o "$usr" = "paper" -o "$usr" = "scissors" ] ; then 
            echo -e "\e[31mYou lose!\e[0m"
        else 
            echo "Write only rock, paper or scissors!"
        fi
    fi    

done