import gymnasium as gym
import gymnasium_robotics
from gymnasium.wrappers import RecordVideo

import matplotlib.pyplot as plt
import numpy as np
import torch

from pathlib import Path

from stable_baselines3 import PPO
from stable_baselines3.common.callbacks import BaseCallback

WORK_DIR = Path.cwd().resolve()

IMG_DIR = WORK_DIR / "report" / "images"
VID_DIR = WORK_DIR / "video"


# Колбэк для сохранения прогресса обучения
class TrainingMonitorCallback(BaseCallback):
    def __init__(self, verbose=0):
        super().__init__(verbose)
        self.rewards = []

    def _on_step(self) -> bool:
        if "rewards" in self.locals:
            self.rewards.append(self.locals["rewards"])
        return True


def main():
    # Создаем среду
    env = gym.make('AdroitHandHammer-v1', render_mode="rgb_array")
    env = RecordVideo(env, str(VID_DIR), episode_trigger=lambda e: True)

    # Проверяем доступность CUDA
    device = "cuda" if torch.cuda.is_available() else "cpu"

    # Создаем модель PPO с указанием устройства
    model = PPO("MlpPolicy", env, verbose=1, device=device)

    # Колбэк для мониторинга
    callback = TrainingMonitorCallback()

    # Обучаем модель
    total_timesteps = 1000000
    print("Начинается обучение модели...")
    model.learn(total_timesteps=total_timesteps, callback=callback)
    print("Обучение завершено.")

    # Сохраняем модель
    model.save("ppo_adroit_hammer")

    # Строим график наград
    plt.figure(figsize=(10, 6))
    plt.plot(np.cumsum(callback.rewards), label="Награды")
    plt.xlabel("Шаги")
    plt.ylabel("Сумма наград")
    plt.title("Процесс обучения")
    plt.legend()
    plt.grid()

    output = IMG_DIR / "training_rewards.png"
    output.parent.mkdir(parents=True, exist_ok=True)
    plt.savefig(output)
    print("График обучения сохранен как 'training_rewards.png'.")

    # Тестируем обученную модель
    obs, _ = env.reset()
    while True:
        action, _ = model.predict(obs, deterministic=True)
        obs, reward, done, truncated, _ = env.step(action)
        print(f"REWARD: {reward}")
        if done or truncated:
            obs, _ = env.reset()
            break


if __name__ == "__main__":
    main()
