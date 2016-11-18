package jp.co.heartsoft.arduino;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import javax.websocket.Session;

public class SessionUtil {

    private static Map<Integer, SessionContainer> containers = new LinkedHashMap<>();

    public static synchronized SessionContainer getContainer(int carNo) {
        return containers.get(carNo);
    }

    public static synchronized void putContainer(int carNo, SessionContainer container) {
        containers.put(carNo, container);
    }

    public static synchronized SessionContainer getContainer(Session session) {
        SessionContainer container = null;
        for (Entry<Integer, SessionContainer> e : containers.entrySet()) {
            if (e.getValue().client == session || e.getValue().arduino == session) {
                container = e.getValue();
                break;
            }
        }

        return container;
    }

    public static synchronized List<Map<Integer, Boolean>> getStatus() {

        List<Map<Integer, Boolean>> statusList = new ArrayList<>();
        for (Entry<Integer, SessionContainer> e : containers.entrySet()) {
            Map<Integer, Boolean> status = new HashMap<>();
            boolean enabled = e.getValue().client == null;
            status.put(e.getKey(), enabled);

            statusList.add(status);
        }

        return statusList;
    }

    public static void remove(SessionContainer container) {
        if (container != null) {
            containers.remove(container.no);
        }

    }
}
