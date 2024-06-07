import { broadcast } from '../utils/broadcast';
import { GameConstants } from '../constants';
import { getPlayers } from './connection';
import { isValidUUID, Request } from '../interfaces/request';
import { Player } from '../interfaces/player';

let gameInterval: NodeJS.Timeout | null = null;
let finished: boolean = false;

export const gameLoop = (): void => {
  broadcast();
};

export const hanldeUpdate = (
  player: Player | undefined,
  request: Request
): void => {
  if (isValidUUID(request.playerId)) {
    if (player && request.type == GameConstants.ACTION) {
      player.direction = request.direction;
      player.score += 1;
      console.log(player.direction, player.score);
    }
  }
};

export const handleStop = (): void => {
  const players = getPlayers();
  const playerCount = players.length;

  if (gameInterval) {
    clearInterval(gameInterval);
    gameInterval = null;
    finished = true;
  }
  if (playerCount === 1 && finished) {
    const winner = players[0];
    winner.socket.write(
      JSON.stringify({ type: "WIN", score: winner.score }) + "\n"
    );
  }
};

export const handleStart = (): void => {
  const players = getPlayers();
  const playerCount = players.length;

  if (playerCount >= GameConstants.MIN_OF_PLAYERS) {
    if (!gameInterval) {
      // Envia para todos os players que o game comecou
      players.forEach(player => player.socket.write('true'))

      // entra no loop do game e avisa os jogadores a cada tick
      gameInterval = setInterval(() => {
        gameLoop();
      }, 1000 / GameConstants.TICKS_PER_SECOND);
    }
  }

  // se tiver 1 player espernado, fica chamando a handleStart para ver quando pode comecar o jogo
  if (playerCount && playerCount == 1) {
    players[0].socket.write('wait')
    setTimeout(handleStart, 1000);
  }
};
