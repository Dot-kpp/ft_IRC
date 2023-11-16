#WeeChat Docker image

To build:
	docker build -t my-weechat-image .

To use:
	docker run -it --rm my-weechat-image


To connect to server:
	
	-Launch Weechat container
	
	-Once inside the container, you'll be at the WeeChat prompt. You can start WeeChat by typing:  /weechat
	
	-Connect to an IRC server:
		weechat ip: "IPAddress": "172.17.0.2",
		WeeChat uses the /server command to connect to IRC servers. For example, to connect to the freenode IRC server, you can type:  /server add freenode chat.freenode.net/6667
		Running on acouture: /server add freenode 10.18.255.255/6667
	
	-Connect to the server:
		After adding the server, you can connect to it using:  /connect freenode

	-Join a channel:
		Once connected, you can join a channel using the /join command. For example:  /join #channel-name

		