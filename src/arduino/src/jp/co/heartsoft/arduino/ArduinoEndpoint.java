package jp.co.heartsoft.arduino;

import java.io.IOException;

import javax.websocket.CloseReason;
import javax.websocket.EndpointConfig;
import javax.websocket.OnClose;
import javax.websocket.OnError;
import javax.websocket.OnMessage;
import javax.websocket.OnOpen;
import javax.websocket.Session;
import javax.websocket.server.ServerEndpoint;

@ServerEndpoint("/arduino")
public class ArduinoEndpoint {

    private static int carNo = 1;

    @OnOpen
    public void onOpen(Session session, EndpointConfig ec) throws IOException {

        Operation o = new Operation();
        o.carNo = carNo++;
        o.speed = 0;
        o.handle = 0;

        SessionContainer container = new SessionContainer();
        container.no = o.carNo;
        container.arduino = session;

        SessionUtil.putContainer(o.carNo, container);

        String json = JsonUtil.toJson(o);
        session.getBasicRemote().sendText(json);
    }

    @OnMessage
    public void onMessage(String msg, Session session) throws IOException {

        // nothing todo
    }

    @OnClose
    public void onClose(Session session, CloseReason reason) throws IOException {
        SessionContainer container = SessionUtil.getContainer(session);
        if (container != null && container.client != null) {
            container.client.close();
            container.client = null;

        }
        SessionUtil.remove(container);
    }

    /*
     * 接続エラーが発生したとき
     */
    @OnError
    public void onError(Session session, Throwable t) throws IOException {
        System.out.println("on error arduino " + session);
        SessionContainer container = SessionUtil.getContainer(session);
        if (container != null && container.client != null) {
            container.client.close();
            container.client = null;

        }
        SessionUtil.remove(container);
    }
}
