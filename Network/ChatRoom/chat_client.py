# TCP Chat client
# http://www.binarytides.com/code-chat-application-server-client-sockets-python/
# http://www.ibm.com/developerworks/linux/tutorials/l-pysocks/

import socket, select, string, sys

def prompt() :
    sys.stdout.write('<You> ')
    sys.stdout.flush()

#main function
if __name__ == "__main__":

    if(len(sys.argv) < 3) :
        print 'Usage : python chat_client.py hostname port'
        sys.exit()

    host = sys.argv[1]
    port = int(sys.argv[2])

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(2)

    # connect to remote host
    try :
        s.connect((host, port))
    except :
        print 'Unable to connect'
        sys.exit()

    print 'Connected to remote host. Start sending messages'
    prompt()

    '''
        The above shown chat client is not going to work on windows.
        It uses the select function to read data from both the socket and the input stream. 
        This works on linux but not on windows.

        File objects on Windows are not acceptable, but sockets are. On Windows,
        the underlying select() function is provided by the WinSock library,
        and does not handle file descriptors that don’t originate from WinSock.
    '''
    while 1:
        socket_list = [sys.stdin, s]

        # Get the list sockets which are readable
        read_sockets, write_sockets, error_sockets = select.select(socket_list , [], [])

        for sock in read_sockets:
            #incoming message from remote server
            if sock == s:
                data = sock.recv(4096)
                if not data :
                    print '\nDisconnected from chat server'
                    sys.exit()
                else :
                    #print data
                    sys.stdout.write(data)
                    prompt()

            #user entered a message
            else :
                msg = sys.stdin.readline()
                s.send(msg)
                prompt()
