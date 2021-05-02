const Utils = require('../utils/Utils');

class Unit {
  id = -1;
  health = 1; // [0%-100%]
  rechargeTime = Utils.getRandomFromSegment(100, 2000); // [100-2000]

  constructor(id) {
    this.id = id;
  }

  getId() {
    return this.id;
  }

  setId(id) {         
    this.id = id;
  }

  getHealth() {
    return this.health;
  }

  setHealth(health) {
    if (health > 1) {
      health = 1;
    } else if (health < 0) {
      health = 0;
    }
    this.health = health;
  }

  getRechargeTime() {
    return this.rechargeTime;
  }

  setRechargeTime(rechargeTime) {
    if (rechargeTime < 100) {
      rechargeTime = 100;
    } else if (rechargeTime > 2000) {
      rechargeTime = 2000;
    }
    this.rechargeTime = rechargeTime;
  }

  isActive() {
    return this.health > 0;
  }

  hurt(damage) {
    this.setHealth(this.health - damage);
    return this.isActive();
  }
}

module.exports = Unit;
