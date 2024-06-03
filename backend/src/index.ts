import * as net from 'net';
import { GameConstants } from "./constants";
import { handleConnection } from './utils/connection';
import { gameLoop } from './utils/game';

const PORT = 8080;

const server = net.createServer((socket) => {
    handleConnection(socket)
});

setInterval(() => {
    gameLoop()
}, 1000 / GameConstants.TICKS_PER_SECOND);


server.listen(PORT, () => console.log(`server listening on port ${PORT}`))