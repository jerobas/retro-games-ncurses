import { broadcast } from "../utils/broadcast";
import { GameConstants } from "../constants";
import { getPlayers } from "./connection";

let gameInterval: NodeJS.Timeout | null = null;
let finished: boolean = false;

export const gameLoop = () => {
  broadcast({ name: "flavio" });
};

export const handleStart = () => {
  let players = getPlayers();
  const playerCount = Object.keys(players).length;

  if (playerCount >= GameConstants.MIN_OF_PLAYERS) {
    if (!gameInterval) {
      gameInterval = setInterval(() => {
        gameLoop();
      }, 1000 / GameConstants.TICKS_PER_SECOND);
    }
  } else {
    if (gameInterval) {
      clearInterval(gameInterval);
      gameInterval = null;
      finished = true;
    }
    if (playerCount === 1 && finished) {
      const winnerId = Object.keys(players)[0];
      const winner = players[winnerId];
      winner.socket.write(
        JSON.stringify({ type: "WIN", score: winner.score }) + "\n"
      );
    }
  }
};
