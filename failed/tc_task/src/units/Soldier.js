const Unit = require('./Unit');

const EXPERIENCE_MAX = 50;

class Soldier extends Unit {
  experience = 0; // [0-50]

  constructor(id) {
    super(id);
  }

  getExperience() {
    return this.experience;
  }

  addExperience() {
    if (this.experience < EXPERIENCE_MAX) {
      ++this.experience;
    }
  }

  getAttackSuccessProbability() {
    return 0.5 * (1 + this.health / 100) * (30 + this.experience + (70 - this.experience) * Math.random()) / 100;
  }

  getInflictDamage() {
    return 0.05 + this.experience / 100;
  }

  isActive() {
    return this.getHealth() > 0;
  }
}

module.exports = Soldier;
