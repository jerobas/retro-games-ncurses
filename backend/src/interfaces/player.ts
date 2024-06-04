import * as net from 'net';

export interface Player {
    id: string;
    snake: number[][];
    direction: string;
    score: number,
    socket: net.Socket
}