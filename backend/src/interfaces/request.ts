import { v4 as uuidV4, validate as validateUuid } from "uuid";

export interface Request {
  type: string;
  playerId: string;
  direction: string;
  snake: number[][];
}

export const isValidUUID = (id: string): boolean => {
  return validateUuid(id);
};
