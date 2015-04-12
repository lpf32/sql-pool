<form method=post action=''>
Message to send: <input type="text" name="message" />
<input type="submit" value="Send" />
</form>
<br />

<?php
if(isset($_POST["message"])) {
    $snd_msg = $_POST["message"];
    $rcv_msg = "";
    $timeout = 10;
    $socket = stream_socket_client('unix:///var/run/mysocket.sock', 
                                    $errorno, $errorstr, $timeout);
    stream_set_timeout($socket, $timeout);

    echo("Sending Message...<br>\n");
    if(!fwrite($socket, $snd_msg))
            echo("Error while writing!!!<br>\n");

    echo("Receiving Message...<br>\n");
    if (!($rcv_msg = fread($socket, 1024))) 
            echo("Error while reading!!!<br>\n");
    else 
            echo($rcv_msg."<br>\n");
    echo("done<br>\n");
}
?>
