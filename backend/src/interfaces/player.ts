import * as net from 'net';

export interface Player {
    playerId: string;
    snake: number[][];
    direction: string;
    score: number,
    socket: net.Socket
}