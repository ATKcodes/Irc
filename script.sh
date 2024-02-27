
(
    echo "PASS p"
    echo "NICK ao"
    echo "USER lazio inter rome"
    for i in {1..20}
    do
        echo "JOIN bella"
        echo "PART bella"
		sleep 0.01
    done
) | nc 127.0.0.1 6667