#!/bin/bash
echo ""
echo ""
echo "Developer: Svetlana Sharkova, TUSUR, Gr. 736-1"
echo "This script runs only if user is root"
echo "Script asks enter user's name to configure this user"
echo "Script allows:"
echo "1. Change password expiration date"
echo "2. Change command shell"
echo "3. Change home directory"
echo ""
echo "-----------------------------"
echo ""
run_user=$(id -Gn | grep -c root)
if [ "$run_user" = "1" ]
then
    while :
    do
        echo "Enter user's name"
        read user
        result=$(grep "$user" /etc/passwd | tr : "\n" | head -1)
        case "$result" in
            "$user" ) echo ""
                echo "User found!\n"
                while :
                do
                    echo "Chose your action:"
                    echo "1 - Set user's password expiration date"
                    echo "2 - Change user's command shell"
                    echo "3 - Change user's home directory"
                    echo "4 - Edit enother user"
                    echo "5 - Exit"
                    echo ""
                    read choice
                    echo ""
                    case "$choice" in
                        "1" ) 
                            while :
                            do
                                echo "Write ammout of days password expires (after last password changing)"
                                read ammountOfDays;
                                echo ""
								echo "Want to continue? ([y]es \ [n]o)"
										read contin
										case "$contin" in
										"Y" | "y" ) echo ""; echo"";;
										"N" | "n" ) echo ""; break;;
										* ) echo "Error input! Try again" 1>&2;;
										esac
                                passwd -x $ammountOfDays $user >/dev/null 2>/dev/null
                                res=$?
                                case "$res" in
                                    "0" ) echo "Done!"
                                        echo ""
                                        break;;
                                    * ) echo "Error input of days! Try again" 1>&2
                                        echo "";;
                                esac
                            done;;
                           
                        "2" ) 
                            while :
                            do
                                echo "Chose command shell:"
                                echo "1 - /bin/sh"
                                echo "2 - /bin/bash"
                                echo "3 - /sbin/nologin"
                                echo ""
                                read shell
								echo "Want to continue? ([y]es \ [n]o)"
										read contin
										case "$contin" in
										"Y" | "y" ) echo ""; echo"";;
										"N" | "n" ) echo ""; break;;
										* ) echo "Error input! Try again" 1>&2;;
										esac
                                echo ""
                                case "$shell" in
                                    "1" ) usermod --shell /bin/sh $user
                                        echo "Command shell for user $user was set to /bin/sh"
                                        echo ""
                                        break;;
                                    "2" ) usermod --shell /bin/bash $user
                                        echo "Command shell for user $user was set to /bin/bash"
                                        echo ""
                                        break;;
                                    "3" ) usermod --shell /sbin/nologin $user
                                        echo "Command shell for user $user was set to /sbin/nologin"
                                        echo ""
                                        break;;
                                    * ) echo "Error input! Try again." 1>&2
                                        echo "";;
                                esac
                            done;;
                            
                        "3" ) 
                            while :
                            do  
								current_home=$(grep $user /etc/passwd | tr : "\n" | tail -2 | head -1)
                                echo "Enter new home directory"
                                echo "Remember to write directory from ./"
                                read newHomeDir
								if [ ${newHomeDir:0:1} != "/" ]
								then echo "Error! Write dierectory from /" 1>&2
								else
								echo "Want to continue? ([y]es \ [n]o)"
										read contin
										case "$contin" in
										"Y" | "y" ) echo ""; echo"";;
										"N" | "n" ) echo ""; break;;
										  * ) echo "Error input! Try again" 1>&2;;
										esac
                                usermod --home $newHomeDir -m $user >/dev/null 2>/dev/null
                                res=$?
                                case "$res" in
                                    "0" ) echo "Done!"
                                        echo ""
                                        break;;
                                    * ) echo "Unable to move home to pointed directory! Try again" 1>&2
                                        usermod --home $current_home $user
                                        echo "";;
                                esac
                                echo ""
							fi
                            done;;
                        "4" ) break;;
                        "5" ) exit $?;;
                        * ) echo "Error input, try again."
                            echo ""
                    esac
                done;;
            * ) echo "User not found, try again"
                echo "";;
        esac
    done
    
else
    echo "You are not root-user!" 1>&2; exit -1;
fi
