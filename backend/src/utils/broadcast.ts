import { getPlayers } from './connection'

export const broadcast = (data: object) => {
    const players = getPlayers()

    for (const playerId in players) {
        players[playerId].socket.write(JSON.stringify({
            type: "broadcast",
            data,
            player: {
                id: playerId,
                snake: players[playerId].snake,
                score: players[playerId].score,
                lastDirectory: players[playerId].direction
            }
        }) + '\n')
    }
}