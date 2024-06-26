import { Player } from "../interfaces/player";
import { handleStart, handleStop, hanldeUpdate } from "./game";
import { v4 as uuidV4 } from "uuid";
import * as net from "net";

const players: Map<string, Player> = new Map();

export const handleConnection = (socket: net.Socket) => {
  const playerId = uuidV4().toString();

  const newPlayer: Player = {
    playerId,
    snake: [],
    direction: "",
    score: 0,
    socket,
  };

  players.set(playerId, newPlayer);

  socket.write(JSON.stringify({ playerId: playerId }));

  socket.on("data", (data: Buffer) => {
    const request = JSON.parse(data.toString());
    const player = players.get(request.playerId);
    hanldeUpdate(player, request);
  });

  socket.on("end", () => {
    players.delete(playerId);
    handleStop()
  });

  handleStart();
};

export const getPlayers = () => {
  return Array.from(players.values());
};
