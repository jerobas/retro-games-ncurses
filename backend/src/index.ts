import * as net from 'net';
import { handleConnection } from './utils/connection';

const PORT = 8080;

const server = net.createServer((socket: net.Socket) => {
  handleConnection(socket);
});

server.listen(PORT, () => console.log(`server listening on port ${PORT}`));
