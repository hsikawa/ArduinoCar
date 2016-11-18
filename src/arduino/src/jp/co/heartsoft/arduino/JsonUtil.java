package jp.co.heartsoft.arduino;

import java.io.IOException;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;

public class JsonUtil {

    private static ObjectMapper mapper = new ObjectMapper();

    public static synchronized String toJson(Object obj) throws JsonProcessingException {
        return mapper.writeValueAsString(obj);
    }

    public static synchronized Operation toObject(String json)
            throws IOException {
        return mapper.readValue(json, Operation.class);
    }

}
