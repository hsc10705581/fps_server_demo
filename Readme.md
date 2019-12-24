# A demo game server

## usage

auth.[cpp/h]
database.[cpp/h]

## 关系图

```mermaid
graph TD
es[epoll_server] --> ctrl[controller]
ctrl --> re[region]
ctrl --> pl[player]
common[common]
```