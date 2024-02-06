
(
    echo "PASS p"
    echo "NICK troia"
    echo "USER dio merda infame"
    for i in {1..20}
    do
        echo "JOIN l"
        echo "PART l"
		sleep 0.01
    done
) | nc 127.0.0.1 6668