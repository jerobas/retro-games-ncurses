import * as net from 'net';
import { Player } from "../interfaces/player"
import { v4 as uuidV4 } from 'uuid'

const players: { [key: string]: Player } = {}

export const handleConnection = (socket: net.Socket) => {
    const playerId = uuidV4().toString()
    players[playerId] = {
        id: playerId,
        snake: [],
        direction: 0,
        score: 0,
        socket
    }

    socket.on('data', (data: string) => {
        const playerInfo = JSON.parse(data)
        if (playerInfo.type == 'MOVE') {
            console.log(playerInfo)
            players[playerId].direction = playerInfo.direction
        }
    });

    socket.on('close', () => {
        delete players[playerId]
    });

    socket.on('error', (err) => {
        console.error(`Error: ${err.message}`);
    });
}

export const getPlayers = () => {
    return players;
}