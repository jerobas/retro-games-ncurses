import * as net from 'net';

export interface Player {
    id: string;
    snake: number[][];
    direction: number;
    score: number,
    socket: net.Socket
}