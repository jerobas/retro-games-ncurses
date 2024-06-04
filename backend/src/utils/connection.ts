import { Player } from "../interfaces/player";
import { handleStart } from "./game";
import { v4 as uuidV4 } from "uuid";
import * as net from "net";

const players: { [key: string]: Player } = {};

export const handleConnection = (socket: net.Socket) => {
  const playerId = uuidV4().toString();

  players[playerId] = {
    id: playerId,
    snake: [],
    direction: '',
    score: 0,
    socket,
  };

  players[playerId].socket.write(JSON.stringify({ playerId: playerId }));

  socket.on("data", (data: string) => {
    const playerInfo = JSON.parse(data);
    if (players[playerInfo.playerId] && playerInfo.type == "MOVE") {
      players[playerInfo.playerId].direction = playerInfo.direction;
      players[playerInfo.playerId].score += 1;
    }
  });

  socket.on("end", () => {
    delete players[playerId];
    handleStart();
  });

  socket.on("error", (err) => {
    console.error(`Error: ${err.message}`);
  });

  handleStart();
};

export const getPlayers = () => {
  return players;
};
