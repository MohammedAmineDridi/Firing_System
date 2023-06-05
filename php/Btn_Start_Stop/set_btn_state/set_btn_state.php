<?php

include "../state_btn_var.php";

// Set the response headers for CORS
header('Access-Control-Allow-Origin: *');

if (isset($_GET['state'])) {
    $_SESSION['btn_state'] = $_GET['state'];
}

?>