<?php

include "../state_btn_var.php";

// Set the response headers for CORS
header('Access-Control-Allow-Origin: *');
header('Content-Type: application/json');

echo json_encode($_SESSION['btn_state']);  

?>