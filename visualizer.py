import sys, os, warnings
os.environ['PYTHONWARNINGS'] = 'ignore::UserWarning'
warnings.simplefilter('ignore')

import matplotlib
matplotlib.use('Qt5Agg')
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import numpy as np

class Viz:
    def __init__(self):
        self.data = {'t': [], 'r': [], 'p': [], 'y': []}
        self.curr = [0, 0, 0]  # roll, pitch, yaw
        
        self.fig = plt.figure(figsize=(12, 8))
        self.ax = [plt.subplot(2,2,i+1, projection='3d' if i==3 else None) for i in range(4)]
        
        for i, (title, lim, col) in enumerate([('Roll', 180, 'b'), ('Pitch', 90, 'r'), ('Yaw', 180, 'g')]):
            self.ax[i].set_title(title)
            self.ax[i].set_ylim(-lim, lim)
            self.ax[i].grid(True)
        
        self.lines = [self.ax[i].plot([], [], c)[0] for i, c in enumerate(['b', 'r', 'g'])]
        self.ax[3].set_title('3D')
        
    def update(self, _):
        try:
            line = sys.stdin.readline().strip().split('\t')
            if line[0] == 'Time': return
            
            t, r, p, y = float(line[0]), float(line[9]), float(line[10]), float(line[11])
            
            for k, v in zip(['t', 'r', 'p', 'y'], [t, r, p, y]):
                self.data[k].append(v)
                if len(self.data[k]) > 100: self.data[k].pop(0)
            
            self.curr = [r, p, y]
            
            if len(self.data['t']) > 1:
                for i, k in enumerate(['r', 'p', 'y']):
                    self.lines[i].set_data(self.data['t'], self.data[k])
                    self.ax[i].set_xlim(self.data['t'][0], self.data['t'][-1])
                
                self.draw_cube()
        except: pass
    
    def draw_cube(self):
        self.ax[3].cla()
        self.ax[3].set_xlim(-2,2); self.ax[3].set_ylim(-2,2); self.ax[3].set_zlim(-2,2)
        
        pts = np.array([[-1,-1,-.3],[1,-1,-.3],[1,1,-.3],[-1,1,-.3],
                        [-1,-1,.3],[1,-1,.3],[1,1,.3],[-1,1,.3]])
        
        r, p, y = [np.radians(a) for a in self.curr]
        
        Rx = [[1,0,0],[0,np.cos(r),-np.sin(r)],[0,np.sin(r),np.cos(r)]]
        Ry = [[np.cos(p),0,np.sin(p)],[0,1,0],[-np.sin(p),0,np.cos(p)]]
        Rz = [[np.cos(y),-np.sin(y),0],[np.sin(y),np.cos(y),0],[0,0,1]]
        
        rot = pts @ (np.array(Rz) @ np.array(Ry) @ np.array(Rx)).T
        
        faces = [[rot[j] for j in f] for f in [[0,1,2,3],[4,5,6,7],[0,1,5,4],[2,3,7,6],[0,3,7,4],[1,2,6,5]]]
        
        cube = Poly3DCollection(faces, alpha=.6, edgecolor='k')
        cube.set_facecolor(['cyan','yellow','red','green','blue','magenta'])
        self.ax[3].add_collection3d(cube)
    
    def run(self):
        ani = FuncAnimation(self.fig, self.update, interval=100, cache_frame_data=False)
        plt.tight_layout()
        plt.show()

if __name__ == '__main__':
    Viz().run()