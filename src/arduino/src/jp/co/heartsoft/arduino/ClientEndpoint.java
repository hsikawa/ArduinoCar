package jp.co.heartsoft.arduino;

import java.io.IOException;
import java.util.List;
import java.util.Map;

import javax.websocket.CloseReason;
import javax.websocket.EndpointConfig;
import javax.websocket.OnClose;
import javax.websocket.OnError;
import javax.websocket.OnMessage;
import javax.websocket.OnOpen;
import javax.websocket.Session;
import javax.websocket.server.ServerEndpoint;

@ServerEndpoint("/client")
public class ClientEndpoint {

    @OnOpen
    public void onOpen(Session session, EndpointConfig ec) throws IOException {

        List<Map<Integer, Boolean>> status = SessionUtil.getStatus();

        String json = JsonUtil.toJson(status);
        session.getBasicRemote().sendText(json);
    }

    @OnMessage
    public void onMessage(String msg, Session session) throws IOException {

        System.out.println("massage from client = " + msg);
        Operation o = JsonUtil.toObject(msg);
        SessionContainer container = SessionUtil.getContainer(o.carNo);
        if (container != null) {
        	container.client = session;
            container.arduino.getBasicRemote().sendText(msg);
        }
    }

    @OnClose
    public void onClose(Session session, CloseReason reason) throws IOException {
        SessionContainer container = SessionUtil.getContainer(session);
        if (container != null) {
            container.client = null;
            Operation o = new Operation();
            o.carNo = container.no;
            o.speed = 0;
            o.handle = 0;

            String json = JsonUtil.toJson(o);
            container.arduino.getBasicRemote().sendText(json);
        }
    }

    /*
     * 接続エラーが発生したとき
     */
    @OnError
    public void onError(Session session, Throwable t) {
        System.out.println("on error client " + session);
    }
}
