import {getPlayers} from './connection'

export const broadcast = (data: object) => {
    const players = getPlayers()

    for (const playerId in players) {
        players[playerId].socket.write(JSON.stringify({
            type: "broadcast",
            data: data,
            playerId: playerId,
            player: players[playerId]
        }))
    }
}