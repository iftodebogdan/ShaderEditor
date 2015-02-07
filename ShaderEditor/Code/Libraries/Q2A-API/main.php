<?php

require_once 'caller.php';

$arguments = $argv;
array_shift($arguments);

$method = array_shift($arguments);

$caller = new Caller;
$response = call_user_func_array(array($caller, $method), $arguments);

if (!empty($response)) {
    print $response;
}
