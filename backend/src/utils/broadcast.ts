import { getPlayers } from "./connection";

export const broadcast = (): void => {
  const players = getPlayers();
  players.forEach((player) => {
    // player.socket.write(
    //   JSON.stringify({
    //     type: "broadcast",
    //     player: {
    //       playerId: player.playerId,
    //       snake: player.snake,
    //       score: player.score,
    //       lastDirectory: player.direction,
    //     },
    //   }) + "\n"
    // );
    player.socket.write('true');
  });
};
