 --poll.lua  

m = mqtt.Client(id, 180, user, pass) 
--last will and testament topic
m:lwt(lwt, "offline", 0, 0)  
m:on("offline", function(conn)   
  --do the subscription business
  print("MQTT reconnecting")
  tmr.stop(0)
  dofile("offline.lua")
 end)  

 -- on publish message receive event  
m:on("message", function(conn, topic, data)  
 if (data == "ON") then
   print("Kick relay")
 else
   print("Invalid, ignoring")
 end
end)  

--do the subscription business
tmr.alarm(0, 1000, 1, function()  
  --check ip status and subscribe
  if wifi.sta.status() == 5 and wifi.sta.getip() ~= nil then
    tmr.stop(0)
    --for secure use m:connect(broker url/ip, 8883, 1, function(conn)
    m:connect(broker, 1883, 0, function(conn)
      tmr.stop(4)
      --door topic
      m:subscribe(top,1, function(conn)
      end)
    end)
  end
end)
