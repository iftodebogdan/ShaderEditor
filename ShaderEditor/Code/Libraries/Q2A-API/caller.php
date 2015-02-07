<?php

/**
 * Class which handles API calls.
 */
class Caller
{
    /**
     * The API URL.
     * 
     * @var string
     */
    protected $apiUrl = 'http://iftodebogdan.dlinkddns.com/popescumihai/question2answer/q2a_api.php';
    
    /**
     * Method used to post a question.
     * 
     * @param string $title
     * @param string $content
     * 
     * @return boolean
     */
    public function postQuestion($title, $content)
    {
        $fields = array(
            'title' => $title,
            'content' => $content,
        );
        $data = http_build_query($fields);
        
        $request = curl_init();
        
        curl_setopt($request, CURLOPT_URL, $this->apiUrl);
        curl_setopt($request, CURLOPT_POST, count($fields));
        curl_setopt($request, CURLOPT_POSTFIELDS, $data);
        curl_setopt($request, CURLOPT_RETURNTRANSFER, true);
        
        $response = curl_exec($request);
        
        curl_close($request);
        
        $decodedResponse = json_decode($response, true);
        if (isset($decodedResponse[0]['best_answer'][0]['content'])) {
            $return = strip_tags($decodedResponse[0]['best_answer'][0]['content']);
        } else {
            $return = "There are no answers to this question!";
        }

        return $return . " Visit <a href='http://iftodebogdan.dlinkddns.com/popescumihai/question2answer/'> this link </a> for more information!";
    }
}

